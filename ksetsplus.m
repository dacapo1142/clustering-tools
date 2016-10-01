function [ collections ] = ksetsplus(g, n, k, varargin)
pnames={'measure','verbose', 'which_cluster'};
dflts={'cohesion',false,[]};
[measure, vervose, which_cluster]=internal.stats.parseArgs(pnames, dflts, varargin{:});

addpath('containers');
if which_cluster
    collections=AdjointSet(n,k,which_cluster);
else
    collections=AdjointSet(n,k);
end
iter_order=1:n;
changed=true;
while changed
    changed=false;
    for vid=iter_order
        min_delta_distance=inf;
        old_cid=collections.which_cluster(vid);
        if collections.csize(old_cid) ==1
            continue;
        end
        
        for cid=1:k
            
            cluster=collections.cluster(cid);
            cluster_size=collections.csize(cid);
            switch measure
                case 'cohesion'
                    delta_distance=g(vid,vid)...
                        -2/cluster_size*sum(g(vid, cluster))...
                        +1/cluster_size^2*sum(sum(g(cluster,cluster)));
                case 'distance'
                    delta_distance=1/cluster_size^2 *...
                        (2*cluster_size*sum(g(vid,cluster))...
                        -sum(sum(g(cluster,cluster))));
            end
            
            if cid==old_cid
                delta_distance=delta_distance*cluster_size/(cluster_size-1);
            else
                delta_distance=delta_distance*cluster_size/(cluster_size+1);
            end
            
            if delta_distance<min_delta_distance
                min_delta_distance=delta_distance;
                best_cid=cid;
            end
        end
        
        if best_cid ~= old_cid
            collections.move(vid, best_cid);
            changed=true;
        end
    end
end
end

